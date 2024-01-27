import pygame
from pygame.locals import Color
import argparse
import sys
import random
import cv2
import numpy as np

def create_image(width, height, output_file, num_frames, text_size, use_random_background, save_as_video, flipflop, frame_rate):
    # Initialize pygame
    pygame.init()

    # Set up the display without showing the window
    screen = pygame.display.set_mode((1, 1), pygame.NOFRAME)

    # Set the alternating background colors
    background_colors = [Color("red"), Color("green"), Color("blue")]
    if flipflop:
        background_colors = [Color("red"), Color("green")]

    if use_random_background:

        # Create a list to store the random background images
        background_images = []

        for base_color in background_colors:
            # Create a surface (image) to draw on
            image_surface = pygame.Surface((width, height))

            # Fill the surface with varying brightness for the base color
            for y in range(height):
                for x in range(width):
                    brightness = random.uniform(0, 1)
                    pixel_color = tuple(int(component * brightness) for component in base_color)
                    image_surface.set_at((x, y), pixel_color)

            background_images.append(image_surface)

    else:
        # Create a surface (image) to draw on with the alternating background colors
        background_images = [pygame.Surface((width, height)) for _ in range(len(background_colors))]
        for i, color in enumerate(background_colors):
            background_images[i].fill(color)

    # Define the video codec and create a VideoWriter object
    if save_as_video:
        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
        video_file_name = f"video_test_{frame_rate}fps.mp4"
        video_writer = cv2.VideoWriter(video_file_name, fourcc, frame_rate, (width, height))

    for frame_num in range(num_frames):
        # Copy the background image for the current frame
        image_surface = background_images[frame_num % len(background_images)].copy()

        # Draw the frame number on the surface
        font = pygame.font.Font(None, text_size)
        text_surface = font.render(str(frame_num + 1), True, (0, 0, 0))
        text_rect = text_surface.get_rect(center=(width // 2, height // 2))
        image_surface.blit(text_surface, text_rect)

        # Save as individual images or add to the video
        if save_as_video:
            # Convert Pygame surface to NumPy array
            image_data = pygame.surfarray.array3d(image_surface)
            image_data = np.transpose(image_data, (1, 0, 2))  # Swap width and height dimensions
            image_data = cv2.cvtColor(image_data, cv2.COLOR_RGB2BGR)  # Convert color format
            video_writer.write(image_data)
        else:
            pygame.image.save(image_surface, f"{output_file}_{frame_num + 1}.bmp")

        # Print frame number, progress, and percentage to standard output without newline
        progress_percentage = (frame_num + 1) / num_frames * 100
        sys.stdout.write(f"\rFrame {frame_num + 1}/{num_frames} processed, {progress_percentage:.2f}% done")
        sys.stdout.flush()

    # Release the VideoWriter object if saving as video
    if save_as_video:
        video_writer.release()
        print(f"\nVideo saved to {video_file_name}")

    # Add a newline at the end after completion
    print()

    # Quit pygame
    pygame.quit()

def main():
    parser = argparse.ArgumentParser(description="Create and save frames or a video using pygame.")
    parser.add_argument("--size", choices=["1k", "4k"], default="1k", help="Image size (1k or 4k)")
    parser.add_argument("--frames", type=int, default=1, help="Number of frames to generate")
    parser.add_argument("--text-size", type=int, default=72, help="Text size for frame numbers")
    parser.add_argument("--rand", action="store_true", help="Generate random color background")
    parser.add_argument("--mpeg", action="store_true", help="Save frames as MPEG-4 video")
    parser.add_argument("--flipflop", action="store_true", help="Use only two colors (red and green)")
    parser.add_argument("--rate", type=int, default=30, help="Frame rate for MPEG video")

    args = parser.parse_args()

    if args.size == "1k":
        image_width, image_height = 1920, 1080  # Standard Full HD resolution
    elif args.size == "4k":
        image_width, image_height = 3840, 2160  # Standard 4K resolution

    output_file_path = f"output_frame_{args.size}"

    create_image(image_width, image_height, output_file_path, args.frames, args.text_size, args.rand, args.mpeg, args.flipflop, args.rate)

if __name__ == "__main__":
    main()
